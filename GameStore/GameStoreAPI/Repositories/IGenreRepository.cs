using GameStoreAPI.Entities;

namespace GameStoreAPI.Repositories;

public interface IGenreRepository
{
    Task<IEnumerable<Genre>> GetAllAsync();
    Task<Genre?> GetByIdAsync(int id);
    Task<bool> ExistsAsync(int id);
} 