using System;
using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;

namespace GameStoreAPI.Mapping;

public static class GameMapping
{
    public static GameDto FromEntityToDto(this Game game)
    {
        return new GameDto(
            Id: game.Id,
            Name: game.Name,
            Genre: game.Genre!.Name,
            Price: game.Price,
            ReleaseDate: game.ReleaseDate
        );
    }

    public static Game FromDtoToEntity(this CreateGameDto gameDto)
    {
        return new Game
        {
            Name = gameDto.Name,
            GenreId
            Id = gameDto.Id,
            Price = gameDto.Price,
            ReleaseDate = gameDto.ReleaseDate
        };
    }
}
