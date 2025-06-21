using Microsoft.AspNetCore.Mvc;
using GameStoreAPI.Dtos;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Services;

namespace GameStoreAPI.Controllers;

[ApiController]
[Route("api/[controller]")]
public class GamesController : ControllerBase
{
    private readonly IGameService _gameService;
    private readonly ILogger<GamesController> _logger;

    public GamesController(IGameService gameService, ILogger<GamesController> logger)
    {
        _gameService = gameService ?? throw new ArgumentNullException(nameof(gameService));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }

    /// <summary>
    /// Gets all games
    /// </summary>
    /// <returns>List of all games</returns>
    [HttpGet]
    public async Task<IActionResult> GetAllGames()
    {
        try
        {
            var gameSummaries = await _gameService.GetAllGamesAsync();
            return Ok(gameSummaries);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving all games");
            return Problem(ex.Message, statusCode: 500);
        }
    }

    /// <summary>
    /// Gets a game by ID
    /// </summary>
    /// <param name="id">The ID of the game</param>
    /// <returns>The game if found, otherwise NotFound</returns>
    [HttpGet("{id:int}")]
    public async Task<IActionResult> GetGameById(int id)
    {
        try
        {
            var game = await _gameService.GetGameByIdAsync(id);

            if (game == null)
            {
                return NotFound();
            }

            return Ok(game);
        }
        catch (ValidationException ex)
        {
            _logger.LogWarning(ex, "Validation error for game ID: {Id}", id);
            return BadRequest(ex.Message);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving game with ID {Id}", id);
            return Problem(ex.Message, statusCode: 500);
        }
    }

    /// <summary>
    /// Creates a new game
    /// </summary>
    /// <param name="createGameDto">The game data to create</param>
    /// <returns>The created game</returns>
    [HttpPost]
    public async Task<IActionResult> CreateGame([FromBody] CreateGameDto createGameDto)
    {
        try
        {
            var createdGame = await _gameService.CreateGameAsync(createGameDto);
            return CreatedAtAction(nameof(GetGameById), new { id = createdGame.Id }, createdGame);
        }
        catch (ArgumentNullException ex)
        {
            _logger.LogWarning(ex, "Null game data provided for creation");
            return BadRequest(ex.Message);
        }
        catch (GenreNotFoundException ex)
        {
            _logger.LogWarning(ex, "Attempted to create game with non-existent genre");
            return BadRequest(ex.Message);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while creating game");
            return Problem(ex.Message, statusCode: 500);
        }
    }

    /// <summary>
    /// Updates an existing game
    /// </summary>
    /// <param name="id">The ID of the game to update</param>
    /// <param name="updateGameDto">The updated game data</param>
    /// <returns>No content if successful, NotFound if game doesn't exist</returns>
    [HttpPut("{id:int}")]
    public async Task<IActionResult> UpdateGame(int id, [FromBody] UpdateGameDto updateGameDto)
    {
        try
        {
            var updatedGame = await _gameService.UpdateGameAsync(id, updateGameDto);
            
            if (updatedGame == null)
            {
                return NotFound();
            }

            return NoContent();
        }
        catch (ArgumentNullException ex)
        {
            _logger.LogWarning(ex, "Null game data provided for update of game ID: {Id}", id);
            return BadRequest(ex.Message);
        }
        catch (ValidationException ex)
        {
            _logger.LogWarning(ex, "Validation error for game ID: {Id}", id);
            return BadRequest(ex.Message);
        }
        catch (GenreNotFoundException ex)
        {
            _logger.LogWarning(ex, "Attempted to update game with non-existent genre");
            return BadRequest(ex.Message);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while updating game with ID {Id}", id);
            return Problem(ex.Message, statusCode: 500);
        }
    }

    /// <summary>
    /// Deletes a game
    /// </summary>
    /// <param name="id">The ID of the game to delete</param>
    /// <returns>No content if successful, NotFound if game doesn't exist</returns>
    [HttpDelete("{id:int}")]
    public async Task<IActionResult> DeleteGame(int id)
    {
        try
        {
            var deleted = await _gameService.DeleteGameAsync(id);
            
            if (!deleted)
            {
                return NotFound();
            }

            return NoContent();
        }
        catch (ValidationException ex)
        {
            _logger.LogWarning(ex, "Validation error for game ID: {Id}", id);
            return BadRequest(ex.Message);
        }
        catch (GameStoreException ex)
        {
            _logger.LogError(ex, "Error occurred while deleting game with ID {Id}", id);
            return Problem(ex.Message, statusCode: 500);
        }
    }
} 