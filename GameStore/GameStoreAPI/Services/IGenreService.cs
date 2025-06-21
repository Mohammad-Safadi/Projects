using GameStoreAPI.Dtos;

namespace GameStoreAPI.Services;

public interface IGenreService
{
    Task<IEnumerable<GenreDto>> GetAllGenresAsync();
    Task<GenreDto?> GetGenreByIdAsync(int id);
} 