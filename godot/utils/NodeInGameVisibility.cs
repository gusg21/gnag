using Godot;
using System;

public partial class NodeInGameVisibility : Node2D
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
