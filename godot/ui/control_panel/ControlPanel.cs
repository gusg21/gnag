using Godot;
using System;
using System.Diagnostics;

public partial class ControlPanel : Control
{
	[Export]
	public BoardState BoardState;

	private Godot.Collections.Array<Node> _planningEnabledControls;

    public override void _EnterTree()
    {
        base._EnterTree();
    }

    public override void _Ready()
    {
		_planningEnabledControls = GetTree().GetNodesInGroup("planning_enabled");

        base._Ready();
    }

    public override void _Process(double delta)
    {
		// Disable nodes not in group "planning_enabled" when outside of planning phase
		bool inPlanningPhase = BoardState.CurrentPhase == BoardPhase.PLAYER_PLANNING;
		foreach (Node control in _planningEnabledControls) {
			// GD.Print(control.Name);
			if (control.HasMethod("set_disabled")) {
				control.Call("set_disabled", !inPlanningPhase);
			}
		}

        base._Process(delta);
    }

    public void OnMoveButtonPressed() {
		BoardState.BeginActionDefinition(new MoveAction(BoardState.SelectedCharacter));
	}
}
