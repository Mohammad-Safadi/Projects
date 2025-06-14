using System;

namespace GameStoreAPI.Endpoints;

using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;
using System.Collections.Generic;
using System.Linq;

public static class GamesEndpoints
{
    const string GetGameEndpointName = "GetGame";
    private static readonly List<GameDto> games = new List<GameDto>
    {
        new GameDto(1, "The Legend of Zelda: Breath of the Wild", "Action-Adventure", 59.99m, new DateOnly(2017, 3, 3)),
        new GameDto(2, "Super Mario Odyssey", "Platformer", 59.99m, new DateOnly(2017, 10, 27)),
        new GameDto(3, "Animal Crossing: New Horizons", "Simulation", 59.99m, new DateOnly(2020, 3, 20))
    };
    
    public static RouteGroupBuilder MapEndpoints(this WebApplication app)
    {
        var group = app
                    .MapGroup("/games")
                    .WithParameterValidation();

        // Get /games
        group.MapGet("/", () => games);

        // Get /games/{id}
        group.MapGet("/{id:int}", (int id) =>
        {
            var game = games.FirstOrDefault(g => g.Id == id);
            return game is not null ? Results.Ok(game) : Results.NotFound();
        })
        .WithName(GetGameEndpointName);

        group.MapPost("/", (CreateGameDto createGameDto, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            Game newGame = new()
            {
                Name = createGameDto.Name,
                Genre = dbContext.Genres.Find(createGameDto.GenreId),
                GenreId = createGameDto.GenreId,
                Price = createGameDto.Price,
                ReleaseDate = createGameDto.ReleaseDate
            };

            dbContext.Games.Add(newGame); // to add the new game to the database
            dbContext.SaveChanges(); // to save changes to the database 

            GameDto newGameDto = new GameDto(
                newGame.Id,
                newGame.Name,
                newGame.Genre!.Name,
                newGame.Price,
                newGame.ReleaseDate
            );

            return Results.CreatedAtRoute(GetGameEndpointName, new { id = newGame.Id }, newGameDto);

        }).WithParameterValidation();

        group.MapPut("/{id}", (int id, UpdateGameDto updateGameDto) =>
        {
            var game = games.FirstOrDefault(g => g.Id == id);
            if (game is null)
            {
                return Results.NotFound();
            }

            game = game with
            {
                Name = updateGameDto.Name ?? game.Name,
                Genre = updateGameDto.Genre ?? game.Genre,
                Price = updateGameDto.Price ?? game.Price,
                ReleaseDate = updateGameDto.ReleaseDate ?? game.ReleaseDate
            };

            games[games.FindIndex(g => g.Id == id)] = game;
            return Results.Ok(game);
        });

        // Delete /games/{id}
        group.MapDelete("/{id:int}", (int id) =>
        {
            var gameIndex = games.FindIndex(g => g.Id == id);
            if (gameIndex == -1)
            {
                return Results.NotFound();
            }

            games.RemoveAt(gameIndex);
            return Results.NoContent();
        });

        return group;
    }
}

