using Godot;
using System;
using System.Diagnostics;

public partial class Tile : Node2D
{
    [Signal]
    public delegate void TileClickedEventHandler(Vector2I tilePos);

    private Sprite2D _selectedGraphics;
    private Vector2I _tilePos;
    private Area2D _hitbox;
    private bool _mouseOver = false;

    public override void _Ready()
    {
        _selectedGraphics = GetNode<Sprite2D>("SelectedGraphics");
        Debug.Assert(_selectedGraphics != null);

        _hitbox = GetNode<Area2D>("Hitbox");
        Debug.Assert(_hitbox != null);

        _tilePos = BoardState.I.Scenario.WorldPosToIsoTilePos(Position);

        _hitbox.MouseEntered += () => {
            _mouseOver = true;
            GD.Print($"Mouse entered {_tilePos}");
        };
        _hitbox.MouseExited += () => {
            _mouseOver = false;
            GD.Print($"Mouse exited {_tilePos}");
        };

        base._Ready();
    }

    public override void _Input(InputEvent @event)
    {
        if (@event is InputEventMouseButton mbEvent) {
            if (mbEvent.Pressed && _mouseOver) {
                EmitSignal(SignalName.TileClicked, _tilePos);
            }
        }

        base._Input(@event);
    }

    public override void _Process(double delta)
    {
        _selectedGraphics.Visible = BoardState.I.SelectedTiles.Contains(_tilePos);

        base._Process(delta);
    }
}
