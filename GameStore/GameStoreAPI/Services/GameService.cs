using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Mapping;
using GameStoreAPI.Repositories;
using Microsoft.Extensions.Logging;

namespace GameStoreAPI.Services;

public class GameService : IGameService
{
    private readonly IGameRepository _gameRepository;
    private readonly IGenreRepository _genreRepository;
    private readonly ILogger<GameService> _logger;

    public GameService(IGameRepository gameRepository, IGenreRepository genreRepository, ILogger<GameService> logger)
    {
        _gameRepository = gameRepository ?? throw new ArgumentNullException(nameof(gameRepository));
        _genreRepository = genreRepository ?? throw new ArgumentNullException(nameof(genreRepository));
        _logger = logger ?? throw new ArgumentNullException(nameof(logger));
    }

    public async Task<IEnumerable<GameSummaryDto>> GetAllGamesAsync()
    {
        try
        {
            var games = await _gameRepository.GetAllAsync();
            var gameSummaries = games.Select(g => g.FromEntityToGameSummaryDto()).ToList();

            _logger.LogInformation("Retrieved {Count} games from database", gameSummaries.Count);
            return gameSummaries;
        }
        catch (Exception ex)
        {
            _logger.LogError(ex, "Error occurred while retrieving all games");
            throw new GameStoreException("Failed to retrieve games from database", ex);
        }
    }

    public async Task<GameDetailsDto?> GetGameByIdAsync(int id)
    {
        try
        {
            if (id <= 0)
            {
                _logger.LogWarning("Invalid game ID provided: {Id}", id);
                throw new ValidationException($"Invalid game ID: {id}");
            }

            var game = await _gameRepository.GetByIdAsync(id);

            if (game == null)
            {
                _logger.LogInformation("Game with ID {Id} not found", id);
                return null;
            }

            var gameDetails = game.FromEntityToGameDetailsDto();
            _logger.LogInformation("Retrieved game with ID {Id}", id);
            return gameDetails;
        }
        catch (Exception ex) when (ex is not ValidationException)
        {
            _logger.LogError(ex, "Error occurred while retrieving game with ID {Id}", id);
            throw new GameStoreException($"Failed to retrieve game with ID {id}", ex);
        }
    }

    public async Task<IEnumerable<GameSummaryDto>> GetGamesByIdsAsync(List<int> ids)
    {
        try
        {
            if (ids == null || !ids.Any())
            {
                _logger.LogWarning("Empty or null list of game IDs provided");
                return Enumerable.Empty<GameSummaryDto>();
            }

            if (ids.Any(id => id <= 0))
            {
                _logger.LogWarning("Invalid game IDs found in the list: {Ids}", string.Join(", ", ids.Where(id => id <= 0)));
                throw new ValidationException("All game IDs must be greater than 0");
            }

            var games = await _gameRepository.GetByIdsAsync(ids);
            var gameSummaries = games.Select(g => g.FromEntityToGameSummaryDto()).ToList();

            _logger.LogInformation("Retrieved {Count} games from database using Dapper", gameSummaries.Count);
            return gameSummaries;
        }
        catch (Exception ex) when (ex is not ValidationException)
        {
            _logger.LogError(ex, "Error occurred while retrieving games by IDs: {Ids}", string.Join(", ", ids));
            throw new GameStoreException("Failed to retrieve games from database", ex);
        }
    }

    public async Task<GameDetailsDto> CreateGameAsync(CreateGameDto createGameDto)
    {
        try
        {
            if (createGameDto == null)
            {
                throw new ArgumentNullException(nameof(createGameDto));
            }

            // Validate that the genre exists
            var genreExists = await _genreRepository.ExistsAsync(createGameDto.GenreId);
            if (!genreExists)
            {
                _logger.LogWarning("Attempted to create game with non-existent genre ID: {GenreId}", createGameDto.GenreId);
                throw new GenreNotFoundException(createGameDto.GenreId);
            }

            var game = createGameDto.FromDtoToEntity();
            var createdGame = await _gameRepository.AddAsync(game);

            var gameDetails = createdGame.FromEntityToGameDetailsDto();
            _logger.LogInformation("Created new game with ID {Id}", createdGame.Id);
            return gameDetails;
        }
        catch (Exception ex) when (ex is not ArgumentNullException && ex is not GenreNotFoundException)
        {
            _logger.LogError(ex, "Error occurred while creating game");
            throw new GameStoreException("Failed to create game", ex);
        }
    }

    public async Task<GameDetailsDto?> UpdateGameAsync(int id, UpdateGameDto updateGameDto)
    {
        try
        {
            if (id <= 0)
            {
                _logger.LogWarning("Invalid game ID provided for update: {Id}", id);
                throw new ValidationException($"Invalid game ID: {id}");
            }

            if (updateGameDto == null)
            {
                throw new ArgumentNullException(nameof(updateGameDto));
            }

            // Validate that the genre exists
            var genreExists = await _genreRepository.ExistsAsync(updateGameDto.GenreId);
            if (!genreExists)
            {
                _logger.LogWarning("Attempted to update game with non-existent genre ID: {GenreId}", updateGameDto.GenreId);
                throw new GenreNotFoundException(updateGameDto.GenreId);
            }

            var game = updateGameDto.ToEntity(id);
            var updatedGame = await _gameRepository.UpdateAsync(id, game);

            if (updatedGame == null)
            {
                _logger.LogInformation("Game with ID {Id} not found for update", id);
                return null;
            }

            var gameDetails = updatedGame.FromEntityToGameDetailsDto();
            _logger.LogInformation("Updated game with ID {Id}", id);
            return gameDetails;
        }
        catch (Exception ex) when (ex is not ValidationException && ex is not ArgumentNullException && ex is not GenreNotFoundException)
        {
            _logger.LogError(ex, "Error occurred while updating game with ID {Id}", id);
            throw new GameStoreException($"Failed to update game with ID {id}", ex);
        }
    }

    public async Task<bool> DeleteGameAsync(int id)
    {
        try
        {
            if (id <= 0)
            {
                _logger.LogWarning("Invalid game ID provided for deletion: {Id}", id);
                throw new ValidationException($"Invalid game ID: {id}");
            }

            var deleted = await _gameRepository.DeleteAsync(id);

            if (!deleted)
            {
                _logger.LogInformation("Game with ID {Id} not found for deletion", id);
                return false;
            }

            _logger.LogInformation("Deleted game with ID {Id}", id);
            return true;
        }
        catch (Exception ex) when (ex is not ValidationException)
        {
            _logger.LogError(ex, "Error occurred while deleting game with ID {Id}", id);
            throw new GameStoreException($"Failed to delete game with ID {id}", ex);
        }
    }
} 