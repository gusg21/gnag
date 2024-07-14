using Godot;
using System;

public partial class Scenario : Node2D
{
    [Signal]
    public delegate void SomeTileClickedEventHandler(Vector2I tilePos);

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
    
    public int Width;
    public int Height;

    public override void _EnterTree()
    {
        base._EnterTree();
    }

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
        Width = data.GridWidth;
        Height = data.GridHeight;

        for (int xx = 0; xx < data.GridWidth; xx++)
        {
            for (int yy = 0; yy < data.GridHeight; yy++)
            {
                Tile tile = TileScene.Instantiate<Tile>();
                tile.Position = IsoTilePosToWorldPos(new Vector2I(
                    xx,
                    yy
                ));
                AddChild(tile);

                tile.TileClicked += (Vector2I tilePos) => {
                    EmitSignal(SignalName.SomeTileClicked, tilePos);
                };
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

    public Vector2 IsoTilePosToWorldPos(Vector2I isoTilePos) {
        return new Vector2(
            (isoTilePos.X - isoTilePos.Y) * TileWidth / 2f,
            (isoTilePos.X + isoTilePos.Y) * TileHeight / 2f
        );
    }

    public Vector2I WorldPosToIsoTilePos(Vector2 worldPos) {
        return new Vector2I(
            Mathf.FloorToInt(worldPos.X / TileWidth + worldPos.Y / TileHeight),
            Mathf.FloorToInt(worldPos.Y / TileHeight - worldPos.X / TileWidth)
        );
    }
}
