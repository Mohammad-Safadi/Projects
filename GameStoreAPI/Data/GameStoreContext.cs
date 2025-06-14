using System;
using System.Collections.Generic;
using GameStoreAPI.Entities;
using Microsoft.EntityFrameworkCore;

namespace GameStoreAPI.Data;

public class GameStoreContext(DbContextOptions<GameStoreContext> options) : DbContext(options)
{
    public DbSet<Game> Games => Set<Game>();
    public DbSet<Genre> Genres => Set<Genre>();

    protected override void OnModelCreating(ModelBuilder modelBuilder)
    {
        
        modelBuilder.Entity<Genre>()
            .HasData(
                new Genre { Id = 1, Name = "Action" },
                new Genre { Id = 2, Name = "Adventure" },
                new Genre { Id = 3, Name = "Role-Playing" },
                new Genre { Id = 4, Name = "Simulation" },
                new Genre { Id = 5, Name = "Strategy" }
            );
    }
}
