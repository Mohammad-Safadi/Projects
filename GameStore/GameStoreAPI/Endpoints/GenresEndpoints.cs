namespace GameStoreAPI.Endpoints;

using GameStoreAPI.Data;
using GameStoreAPI.Dtos;
using GameStoreAPI.Entities;
using GameStoreAPI.Mapping;
using Microsoft.EntityFrameworkCore;

public static class GenresEndpoints
{
    public static RouteGroupBuilder MapGenreEndpoints(this WebApplication app)
    {
        var group = app
                    .MapGroup("/genres")
                    .WithParameterValidation();

        // Get /genres
        group.MapGet("/", async (GameStoreContext dbContext) =>
        {
            var genres = await dbContext.Genres
                .Select(g => g.FromEntityToGenreDto())
                .AsNoTracking()
                .ToListAsync();

            return Results.Ok(genres);
        });

        // Get /genres/{id}
        group.MapGet("/{id:int}", async (int id, GameStoreContext dbContext) =>
        {
            var genre = await dbContext.Genres.FindAsync(id);

            return genre is not null
                ? Results.Ok(genre.FromEntityToGenreDto())
                : Results.NotFound();
        });

        return group;
    }
}