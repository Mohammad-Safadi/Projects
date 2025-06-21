using System;

namespace GameStoreAPI.Endpoints;

using GameStoreAPI.Dtos;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Services;
using Microsoft.AspNetCore.Http.HttpResults;

public static class GamesEndpoints
{
    const string GetGameEndpointName = "GetGame";

    public static RouteGroupBuilder MapEndpoints(this WebApplication app)
    {
        var group = app
                    .MapGroup("/games")
                    .WithParameterValidation();

        // Get /games
        group.MapGet("/", async (IGameService gameService) => 
        {
            try
            {
                var gameSummaries = await gameService.GetAllGamesAsync();
                return Results.Ok(gameSummaries);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        });

        // Get /games/{id}
        group.MapGet("/{id:int}", async (int id, IGameService gameService) =>
        {
            try
            {
                var game = await gameService.GetGameByIdAsync(id);

                return game is not null
                    ? Results.Ok(game)
                    : Results.NotFound();
            }
            catch (ValidationException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        })
        .WithName(GetGameEndpointName);

        group.MapPost("/", async (CreateGameDto createGameDto, IGameService gameService) =>
        {
            try
            {
                var createdGame = await gameService.CreateGameAsync(createGameDto);
                return Results.CreatedAtRoute(GetGameEndpointName, new { id = createdGame.Id }, createdGame);
            }
            catch (ArgumentNullException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GenreNotFoundException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        }).WithParameterValidation();

        group.MapPut("/{id}", async (int id, UpdateGameDto updateGameDto, IGameService gameService) =>
        {
            try
            {
                var updatedGame = await gameService.UpdateGameAsync(id, updateGameDto);
                
                if (updatedGame == null)
                {
                    return Results.NotFound();
                }

                return Results.NoContent();
            }
            catch (ArgumentNullException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (ValidationException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GenreNotFoundException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        });

        // Delete /games/{id}
        group.MapDelete("/{id:int}", async (int id, IGameService gameService) =>
        {
            try
            {
                var deleted = await gameService.DeleteGameAsync(id);
                
                if (!deleted)
                {
                    return Results.NotFound();
                }

                return Results.NoContent();
            }
            catch (ValidationException ex)
            {
                return Results.BadRequest(ex.Message);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        });

        return group;
    }
}

