using Microsoft.AspNetCore.Mvc;
using GameStoreAPI.Dtos;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Services;

namespace GameStoreAPI.Controllers;

[ApiController]
[Route("api/[controller]")]
public class GenresController : ControllerBase
{
    private readonly IGenreService _genreService;
    private readonly ILogger<GenresController> _logger;

    public GenresController(IGenreService genreService, ILogger<GenresController> logger)
    {
        _genreService = genreService ?? throw new ArgumentNullException(nameof(genreService));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }
//yhjy
    /// <summary>
    /// Gets all genres
    /// </summary>
    /// <returns>List of all genres</returns>
    [HttpGet]
    public async Task<IActionResult> GetAllGenres()
    {
        try
        {
            var genres = await _genreService.GetAllGenresAsync();
            return Ok(genres);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving all genres");
            return Problem(ex.Message, statusCode: 500);
        }
    }

    /// <summary>
    /// Gets a genre by ID
    /// </summary>
    /// <param name="id">The ID of the genre</param>
    /// <returns>The genre if found, otherwise NotFound</returns>
    [HttpGet("{id:int}")]
    public async Task<IActionResult> GetGenreById(int id)
    {
        try
        {
            var genre = await _genreService.GetGenreByIdAsync(id);

            if (genre == null)
            {
                return NotFound();
            }

            return Ok(genre);
        }
        catch (ValidationException ex)
        {
            _logger.LogWarning(ex, "Validation error for genre ID: {Id}", id);
            return BadRequest(ex.Message);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving genre with ID {Id}", id);
            return Problem(ex.Message, statusCode: 500);
        }
    }
} 