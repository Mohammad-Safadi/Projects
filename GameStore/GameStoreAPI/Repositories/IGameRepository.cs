using GameStoreAPI.Entities;

namespace GameStoreAPI.Repositories;

public interface IGameRepository
{
    Task<IEnumerable<Game>> GetAllAsync();
    Task<Game?> GetByIdAsync(int id);
    Task<Game> AddAsync(Game game);
    Task<Game?> UpdateAsync(int id, Game game);
    Task<bool> DeleteAsync(int id);
    Task<bool> ExistsAsync(int id);
} 