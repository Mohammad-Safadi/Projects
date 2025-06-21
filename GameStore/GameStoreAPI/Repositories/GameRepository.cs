using GameStoreAPI.Data;
using GameStoreAPI.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;

namespace GameStoreAPI.Repositories;

public class GameRepository : IGameRepository
{
    private readonly GameStoreContext _context;
    private readonly ILogger<GameRepository> _logger;

    public GameRepository(GameStoreContext context, ILogger<GameRepository> logger)
    {
        _context = context ?? throw new ArgumentNullException(nameof(context));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
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