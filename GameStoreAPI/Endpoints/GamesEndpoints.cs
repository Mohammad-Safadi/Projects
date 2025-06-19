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
        group.MapGet("/", async (GameStoreContext dbContext) => 
        {
            var gameSummaries = await  dbContext.Games
            .Include(g => g.Genre) // otherwise Genre will be null
            .Select(g => g.FromEntityToGameSummaryDto())
            .AsNoTracking() //used to improve performance by not tracking changes since the entities are not modified
            // and used to read data only
            .ToListAsync();

            return Results.Ok(gameSummaries);
        });

        // Get /games/{id}
        group.MapGet("/{id:int}", async  (int id, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            var game = await dbContext.Games.FindAsync(id);

            return game is not null
                ? Results.Ok(game.FromEntityToGameDetailsDto())
                : Results.NotFound();
        })
        
        .WithName(GetGameEndpointName);

        group.MapPost("/", async (CreateGameDto createGameDto, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            Game mappedGame = createGameDto.FromDtoToEntity();

            dbContext.Games.Add(mappedGame);
            await dbContext.SaveChangesAsync(); // Save changes to the database 

            return Results.CreatedAtRoute(GetGameEndpointName, new { id = mappedGame.Id }, mappedGame.FromEntityToGameDetailsDto());

        }).WithParameterValidation();

        group.MapPut("/{id}", async (int id, UpdateGameDto updateGameDto, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            var existingGame = await  dbContext.Games.FindAsync(id);
            if (existingGame is null)
            {
                return Results.NotFound();
            }

            dbContext.Entry(existingGame)
                        .CurrentValues
                        .SetValues(updateGameDto.ToEntity(id));

            await dbContext.SaveChangesAsync();

            return Results.NoContent();
        });

        // Delete /games/{id}
        group.MapDelete("/{id:int}", async (int id, GameStoreAPI.Data.GameStoreContext dbContext) =>
        {
            await dbContext.Games.Where(g => g.Id == id)
            .ExecuteDeleteAsync(); // ExecuteDelete is used to delete the entity without loading it into memory

            // No need to call dbContext.SaveChangesAsync() after ExecuteDeleteAsync()
            // because ExecuteDeleteAsync() executes the command immediately.

            return Results.NoContent();
        });

        return group;
    }
}

