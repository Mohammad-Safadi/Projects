namespace GameStoreAPI.Endpoints;

using GameStoreAPI.Dtos;
using GameStoreAPI.Exceptions;
using GameStoreAPI.Services;

public static class GenresEndpoints
{
    public static RouteGroupBuilder MapGenreEndpoints(this WebApplication app)
    {
        var group = app
                    .MapGroup("/genres")
                    .WithParameterValidation();

        // Get /genres
        group.MapGet("/", async (IGenreService genreService) =>
        {
            try
            {
                var genres = await genreService.GetAllGenresAsync();
                return Results.Ok(genres);
            }
            catch (GameStoreException ex)
            {
                return Results.Problem(ex.Message, statusCode: 500);
            }
        });

        // Get /genres/{id}
        group.MapGet("/{id:int}", async (int id, IGenreService genreService) =>
        {
            try
            {
                var genre = await genreService.GetGenreByIdAsync(id);

                return genre is not null
                    ? Results.Ok(genre)
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
        });

        return group;
    }
}