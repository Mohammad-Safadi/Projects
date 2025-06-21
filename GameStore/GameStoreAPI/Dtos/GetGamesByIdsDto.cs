using System.ComponentModel.DataAnnotations;

namespace GameStoreAPI.Dtos;

public record class GetGamesByIdsDto(
    [Required]
    [MinLength(1, ErrorMessage = "At least one ID is required")]
    List<int> Ids
); 