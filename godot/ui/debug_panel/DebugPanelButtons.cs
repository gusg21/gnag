using Godot;
using System;
using System.Linq;
using System.Reflection;

public partial class DebugPanelButtons : Control
{
	public DebugPanelButtons I;

	// Called when the node enters the scene tree for the first time.
	public override void _EnterTree()
	{
		I = this;
	}
}
