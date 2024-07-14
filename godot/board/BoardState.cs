using Godot;
using System;
using System.Collections.Generic;
using System.Diagnostics;

public enum BoardPhase
{
	PLAYER_PLANNING, // Choosing a player
	PLAYER_DEFINING_ACTION, // Defining an action
	PLAYER_EXECUTING, // Executing the action queue
	ENEMY_PLANNING, ENEMY_EXECUTING
}

public partial class BoardState : Node
{
	public static BoardState I = null;

	public BoardPhase CurrentPhase = BoardPhase.PLAYER_PLANNING;
	public event Action<BoardPhase> CurrentPhaseChanged;

	public IBoardAction CurrentAction = null; // Only valid in certain phases
	public Godot.Collections.Array<Vector2I> SelectedTiles = new();
	[Export]
	public NodePath ScenarioNodePath;
	public Scenario Scenario;

	public Character SelectedCharacter => Characters[SelectedCharacterIndex];
	public List<Character> Characters = new();
	public int SelectedCharacterIndex = 0;

	// Called when the node enters the scene tree for the first time.
	public override void _EnterTree()
	{
		Scenario = GetNode<Scenario>(ScenarioNodePath);
		Debug.Assert(Scenario != null);
		Scenario.SomeTileClicked += (tilePos) => {
			SelectedTiles.Add(tilePos);
		};

		if (I == null)
		{
			I = this;
		}
		else
		{
			QueueFree();
		}

		base._EnterTree();
	}

	public override void _Ready()
	{
		CallDeferred("ChangeBoardPhase", (int)BoardPhase.PLAYER_PLANNING);

		base._Ready();
	}

	public override void _ExitTree()
	{
		I = null;

		base._ExitTree();
	}

	public void RegisterCharacter(Character character)
	{
		Characters.Add(character);
	}

	public void ChangeBoardPhase(BoardPhase newPhase)
	{
		CurrentPhase = newPhase;
		CurrentPhaseChanged?.Invoke(CurrentPhase);
	}

	public void BeginActionDefinition(IBoardAction action)
	{
		GD.Print("Action begun!");
		CurrentAction = action;
		ChangeBoardPhase(BoardPhase.PLAYER_DEFINING_ACTION);
	}

	public void SelectAllTiles()
	{
		for (int xx = 0; xx < Scenario.Width; xx++)
		{
			for (int yy = 0; yy < Scenario.Height; yy++)
			{
				Vector2I tilePos = new(xx, yy);
				if (!SelectedTiles.Contains(tilePos))
				{
					SelectedTiles.Add(tilePos);
				}
			}
		}
	}
}
