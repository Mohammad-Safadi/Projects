using GameStoreAPI.Data;
using GameStoreAPI.Entities;
using Microsoft.EntityFrameworkCore;
using Microsoft.Extensions.Logging;

namespace GameStoreAPI.Repositories;

public class GenreRepository : IGenreRepository
{
    private readonly GameStoreContext _context;
    private readonly ILogger<GenreRepository> _logger;

    public GenreRepository(GameStoreContext context, ILogger<GenreRepository> logger)
    {
        _context = context ?? throw new ArgumentNullException(nameof(context));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }

    public async Task<IEnumerable<Genre>> GetAllAsync()
    {
        return await _context.Genres
            .AsNoTracking()
            .ToListAsync();
    }

    public async Task<Genre?> GetByIdAsync(int id)
    {
        return await _context.Genres
            .AsNoTracking()
            .FirstOrDefaultAsync(g => g.Id == id);
    }

    public async Task<bool> ExistsAsync(int id)
    {
        return await _context.Genres.AnyAsync(g => g.Id == id);
    }
} 