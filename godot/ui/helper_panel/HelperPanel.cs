using Godot;
using System;
using System.Diagnostics;

public partial class HelperPanel : Control
{
	private RichTextLabel _helperLabel;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
		_helperLabel = GetNode<RichTextLabel>("VBoxContainer/PanelContainer/MarginContainer/RichTextLabel");
		Debug.Assert(_helperLabel != null);

		Hide();

		BoardState.I.CurrentPhaseChanged += OnBoardStateCurrentPhaseChanged;
	}

    private void OnBoardStateCurrentPhaseChanged(BoardPhase phase)
    {
        if (phase == BoardPhase.PLAYER_DEFINING_ACTION) {
			Show();
			_helperLabel.Text = BoardState.I.CurrentAction.GetHelperInfo();
		} else {
			Hide();
		}
    }
}
