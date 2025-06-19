using GameStoreAPI.Entities;
using GameStoreAPI.Dtos;

namespace GameStoreAPI.Mapping;

// mapping genre to genre dto
public static class GenreMapping
{
    // mapping genre entity to genre dto
    public static GenreDto FromEntityToGenreDto(this Genre genre)
    {
        return new GenreDto(
            Id: genre.Id,
            Name: genre.Name
        );
    }
    // mapping genre dto to genre entity
    public static Genre FromDtoToEntity(this GenreDto genreDto)
    {
        return new Genre
        {
            Id = genreDto.Id,
            Name = genreDto.Name
        };
    }
}