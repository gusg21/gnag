using Godot;
using System;

public partial class BoardCamera : Camera2D
{
	[Export]
	public float CenterXRatio = 0.5f;

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
		GlobalPosition = GlobalPosition.Lerp(BoardState.I.SelectedCharacter.GlobalPosition, (float)delta * 5f);

		Offset = new(
			GetViewport().GetVisibleRect().Size.X * CenterXRatio,
			0f
		);
	}
}
