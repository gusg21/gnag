using Godot;
using System;

public partial class Scenario : Node2D
{
    [Export(PropertyHint.File, "*.tscn")]
    public PackedScene TileScene;
    [Export]
    public int TileWidth;
    [Export]
    public int TileHeight;
    [Export(PropertyHint.ResourceType, "ScenatioData")]
    public ScenarioData InitialData;

    public override void _Ready()
    {
        base._Ready();

        if (InitialData != null)
        {
            CreateScenario(InitialData);
        }
    }

    public void CreateScenario(ScenarioData data)
    {
        for (int xx = 0; xx < data.GridWidth; xx++)
        {
            for (int yy = 0; yy < data.GridHeight; yy++)
            {
                Tile tile = TileScene.Instantiate<Tile>();
                tile.Position = IsoTilePosToWorldPos(new Vector2(
                    xx,
                    yy
                ));
                tile.Position = new Vector2(
                    tile.Position.X * TileWidth / 2f,
                    tile.Position.Y * TileHeight / 2f
                );
                AddChild(tile);
            }
        }
    }

    private Vector2 IsoTilePosToWorldPos(Vector2 isoTilePos) {
        return new Vector2(
            isoTilePos.X - isoTilePos.Y,
            isoTilePos.X + isoTilePos.Y
        );
    }
}
