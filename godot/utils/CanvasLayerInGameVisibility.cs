using Godot;
using System;

public partial class CanvasLayerInGameVisibility : CanvasLayer
{
	[Export]
	public bool VisibleInGame = true;

    public override void _EnterTree()
    {
		if (VisibleInGame) {
			Show();
			Visible = true;
		} else {
			Hide();
			Visible = false;
		}

        base._EnterTree();
    }
}
