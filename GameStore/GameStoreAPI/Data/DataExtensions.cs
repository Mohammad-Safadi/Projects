using Microsoft.EntityFrameworkCore;

namespace GameStoreAPI.Data;

public static class DataExtensions
{
    public static async Task MigrateDbAsync(this WebApplication app)
    {
        using var scope = app.Services.CreateScope();
        var context =  scope.ServiceProvider.GetRequiredService<GameStoreContext>();
        await context.Database.MigrateAsync();
    }
}