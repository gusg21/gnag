using Godot;
using System;

public partial class Scenario : Node2D
{
    [Export(PropertyHint.File, "*.tscn")]
    public PackedScene TileScene;
    [Export(PropertyHint.File, "*.tscn")]
    public PackedScene CharacterScene;
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
                AddChild(tile);
            }
        }

        foreach (var instance in InitialData.CharacterInstances) 
        {
            Character character = CharacterScene.Instantiate<Character>();
            character.Type = instance.CharacterType;
            character.Position = IsoTilePosToWorldPos(instance.TilePos);
            AddChild(character);
        }
    }

    private Vector2 IsoTilePosToWorldPos(Vector2 isoTilePos) {
        return new Vector2(
            (isoTilePos.X - isoTilePos.Y) * TileWidth / 2f,
            (isoTilePos.X + isoTilePos.Y) * TileHeight / 2f
        );
    }
}
