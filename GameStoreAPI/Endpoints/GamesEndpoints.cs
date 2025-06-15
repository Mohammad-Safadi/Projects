using System;

namespace GameStoreAPI.Endpoints;

using GameStoreAPI.Data;
using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;
using GameStoreAPI.Mapping;
using Microsoft.AspNetCore.Http.HttpResults;
using Microsoft.EntityFrameworkCore;
using System.Collections.Generic;
using System.Linq;

public static class GamesEndpoints
{
    const string GetGameEndpointName = "GetGame";

    public static RouteGroupBuilder MapEndpoints(this WebApplication app)
    {
        var group = app
                    .MapGroup("/games")
                    .WithParameterValidation();

        // Get /games
        group.MapGet("/", (GameStoreContext dbContext) =>
        {
            var gameSummaries = dbContext.Games
            .Include(g => g.Genre)
            .Select(g => g.FromEntityToGameSummaryDto());

            var sql = gameSummaries.ToQueryString();
            Console.WriteLine($"SQL Query: {sql}");
            return Results.Ok(gameSummaries.ToList());
        });

        // Get /games/{id}
        group.MapGet("/{id:int}", (int id, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            var game = dbContext.Games.Find(id);

            return game is not null
                ? Results.Ok(game.FromEntityToGameDetailsDto())
                : Results.NotFound();
        })
        
        .WithName(GetGameEndpointName);

        group.MapPost("/", (CreateGameDto createGameDto, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            Game mappedGame = createGameDto.FromDtoToEntity();

            dbContext.Games.Add(mappedGame);
            dbContext.SaveChanges(); // to save changes to the database 

            return Results.CreatedAtRoute(GetGameEndpointName, new { id = mappedGame.Id }, mappedGame.FromEntityToGameDetailsDto());

        }).WithParameterValidation();

        group.MapPut("/{id}", (int id, UpdateGameDto updateGameDto, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            var existingGame = dbContext.Games.Find(id);
            if (existingGame is null)
            {
                return Results.NotFound();
            }

            dbContext.Entry(existingGame)
                        .CurrentValues
                        .SetValues(updateGameDto.ToEntity(id));

            dbContext.SaveChanges();

            return Results.NoContent();
        });

        // Delete /games/{id}
        group.MapDelete("/{id:int}", (int id, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            var existingGame = dbContext.Games.Find(id);
            if (existingGame is null)
            {
                return Results.NotFound();
            }

            dbContext.Games.Remove(existingGame);

            dbContext.SaveChanges();

            return Results.NoContent();
        });

        return group;
    }
}

