using Godot;
using System;

public partial class Bouncer : Node2D
{
	[Export]
	public float Range = 3f;
	[Export]
	public Vector2 Direction = Vector2.Up;
	[Export]
	public float Rate = 2f;
	[Export]
	public bool Discrete = true;
	[Export]
	public float DiscreteSize = 3f;

	private Vector2 _internalOffset = Vector2.Zero;

	// Called when the node enters the scene tree for the first time.
	public override void _Ready()
	{
	}

	// Called every frame. 'delta' is the elapsed time since the previous frame.
	public override void _Process(double delta)
	{
		_internalOffset = Direction * (MathF.Sin(MathF.Tau * Rate * (Time.GetTicksMsec() / 1000f)) * Range);
		Position = Discrete ? _internalOffset.Snapped(Vector2.One * DiscreteSize) : _internalOffset;
	}
}
