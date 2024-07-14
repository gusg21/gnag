using Godot;
using System;

public partial class DebugInfoText : RichTextLabel
{

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
		Text = $"State: {BoardState.I.CurrentPhase}";
	}
}
