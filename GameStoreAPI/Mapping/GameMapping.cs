using System;
using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;

namespace GameStoreAPI.Mapping;

public static class GameMapping
{
    public static GameSummaryDto FromEntityToGameSummaryDto(this Game game)
    {
        return new GameSummaryDto(
            Id: game.Id,
            Name: game.Name,
            Genre: game.Genre!.Name,
            Price: game.Price,
            ReleaseDate: game.ReleaseDate
        );
    }
    public static GameDetailsDto FromEntityToGameDetailsDto(this Game game)
    {
        return new GameDetailsDto(
            Id: game.Id,
            Name: game.Name,
            GenreId: game.GenreId,
            Price: game.Price,
            ReleaseDate: game.ReleaseDate
        );
    }

    public static Game FromDtoToEntity(this CreateGameDto gameDto)
    {
        return new Game
        {
            Name = gameDto.Name,
            GenreId = gameDto.GenreId,
            Price = gameDto.Price,
            ReleaseDate = gameDto.ReleaseDate
        };
    }
}
