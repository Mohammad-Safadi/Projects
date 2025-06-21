using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Mapping;
using GameStoreAPI.Repositories;
using Microsoft.Extensions.Logging;

namespace GameStoreAPI.Services;

public class GenreService : IGenreService
{
    private readonly IGenreRepository _genreRepository;
    private readonly ILogger<GenreService> _logger;

    public GenreService(IGenreRepository genreRepository, ILogger<GenreService> logger)
    {
        _genreRepository = genreRepository ?? throw new ArgumentNullException(nameof(genreRepository));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }

    public async Task<IEnumerable<GenreDto>> GetAllGenresAsync()
    {
        try
        {
            var genres = await _genreRepository.GetAllAsync();
            var genreDtos = genres.Select(g => g.FromEntityToGenreDto()).ToList();

            _logger.LogInformation("Retrieved {Count} genres from database", genreDtos.Count);
            return genreDtos;
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving all genres");
            throw new GameStoreException("Failed to retrieve genres from database", ex);
        }
    }

    public async Task<GenreDto?> GetGenreByIdAsync(int id)
    {
        try
        {
            if (id <= 0)
            {
                _logger.LogWarning("Invalid genre ID provided: {Id}", id);
                throw new ValidationException($"Invalid genre ID: {id}");
            }

            var genre = await _genreRepository.GetByIdAsync(id);

            if (genre == null)
            {
                _logger.LogInformation("Genre with ID {Id} not found", id);
                return null;
            }

            var genreDto = genre.FromEntityToGenreDto();
            _logger.LogInformation("Retrieved genre with ID {Id}", id);
            return genreDto;
        }
        catch (Exception ex) when (ex is not ValidationException)
        {
            _logger.LogError(ex, "Error occurred while retrieving genre with ID {Id}", id);
            throw new GameStoreException($"Failed to retrieve genre with ID {id}", ex);
        }
    }
} 