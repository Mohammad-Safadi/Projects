using GameStoreAPI.Data;
using GameStoreAPI.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;
using Dapper;
using Microsoft.Data.Sqlite;
using System;

namespace GameStoreAPI.Repositories;

public class GameRepository : IGameRepository
{
    private readonly GameStoreContext _context;
    private readonly ILogger<GameRepository> _logger;
    private readonly string _connectionString;

    // Private DTO for Dapper queries to handle DateOnly mapping
    private class GameDapperDto
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
        public int GenreId { get; set; }
        public decimal Price { get; set; }
        public string ReleaseDate { get; set; } = string.Empty; // SQLite stores as TEXT
    }

    // Private DTO for Genre in Dapper queries
    private class GenreDapperDto
    {
        public int Id { get; set; }
        public string Name { get; set; } = string.Empty;
    }

    public GameRepository(GameStoreContext context, ILogger<GameRepository> logger)
    {
        _context = context ?? throw new ArgumentNullException(nameof(context));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
        _connectionString = context.Database.GetConnectionString() ?? 
            throw new InvalidOperationException("Connection string not found");
    }

    public async Task<IEnumerable<Game>> GetAllAsync()
    {
        return await _context.Games
            .Include(g => g.Genre)
            .AsNoTracking()
            .ToListAsync();
    }

    public async Task<Game?> GetByIdAsync(int id)
    {
        return await _context.Games
            .Include(g => g.Genre)
            .AsNoTracking()
            .FirstOrDefaultAsync(g => g.Id == id);
    }

    public async Task<IEnumerable<Game>> GetByIdsAsync(List<int> ids)
    {
        if (ids == null || ids.Count == 0)
            return Enumerable.Empty<Game>();

        using var connection = new SqliteConnection(_connectionString);
        await connection.OpenAsync();

        const string sql = @"
            SELECT g.Id, g.Name, g.GenreId, g.Price, g.ReleaseDate,
                   gen.Id AS Genre_Id, gen.Name AS Genre_Name
            FROM Games g
            INNER JOIN Genres gen ON g.GenreId = gen.Id
            WHERE g.Id IN @Ids";

        var gameCache = new Dictionary<int, Game>();

        await connection.QueryAsync<GameDapperDto, GenreDapperDto, Game>(
            sql,
            (gameDto, genreDto) => MapGameWithGenre(gameDto, genreDto, gameCache),
            new { Ids = ids },
            splitOn: "Genre_Id"
        );

        return gameCache.Values;
    }

    private Game MapGameWithGenre(
        GameDapperDto gameDto,
        GenreDapperDto genreDto,
        Dictionary<int, Game> cache)
    {
        if (!cache.TryGetValue(gameDto.Id, out var cachedGame))
        {
            var newGame = new Game
            {
                Id = gameDto.Id,
                Name = gameDto.Name,
                GenreId = gameDto.GenreId,
                Price = gameDto.Price,
                ReleaseDate = DateOnly.Parse(gameDto.ReleaseDate),
                Genre = new Genre
                {
                    Id = genreDto.Id,
                    Name = genreDto.Name
                }
            };

            cache[gameDto.Id] = newGame;
        }

        return cache[gameDto.Id];
    }

    public async Task<Game> AddAsync(Game game)
    {
        _context.Games.Add(game);
        await _context.SaveChangesAsync();
        return game;
    }

    public async Task<Game?> UpdateAsync(int id, Game game)
    {
        var existingGame = await _context.Games.FindAsync(id);
        if (existingGame == null)
        {
            return null;
        }

        existingGame.Name = game.Name;
        existingGame.GenreId = game.GenreId;
        existingGame.Price = game.Price;
        existingGame.ReleaseDate = game.ReleaseDate;

        await _context.SaveChangesAsync();
        return existingGame;
    }

    public async Task<bool> DeleteAsync(int id)
    {
        var deletedCount = await _context.Games
            .Where(g => g.Id == id)
            .ExecuteDeleteAsync();

        return deletedCount > 0;
    }

    public async Task<bool> ExistsAsync(int id)
    {
        return await _context.Games.AnyAsync(g => g.Id == id);
    }
} 