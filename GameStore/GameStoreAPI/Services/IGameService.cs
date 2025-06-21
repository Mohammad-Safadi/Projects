using GameStoreAPI.Dtos;

namespace GameStoreAPI.Services;

public interface IGameService
{
    Task<IEnumerable<GameSummaryDto>> GetAllGamesAsync();
    Task<GameDetailsDto?> GetGameByIdAsync(int id);
    Task<IEnumerable<GameSummaryDto>> GetGamesByIdsAsync(List<int> ids);
    Task<GameDetailsDto> CreateGameAsync(CreateGameDto createGameDto);
    Task<GameDetailsDto?> UpdateGameAsync(int id, UpdateGameDto updateGameDto);
    Task<bool> DeleteGameAsync(int id);
} 