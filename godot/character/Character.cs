using Godot;
using System;
using System.Diagnostics;

public partial class Character : Node2D
{
    public static readonly string IDLE_FRONT_ANIM_NAME = "idle_front";
    public static readonly string IDLE_BACK_ANIM_NAME = "idle_back";
    public static readonly string WALK_FRONT_ANIM_NAME = "walk_front";
    public static readonly string WALK_BACK_ANIM_NAME = "walk_back";

    [Export(PropertyHint.ResourceType, "CharacterType")]
    public CharacterType Type;

    private AnimatedSprite2D Graphics;
    private Label NameLabel;

    public override void _Ready()
    {
        Debug.Assert(Type != null);

        Graphics = GetNode<AnimatedSprite2D>("Graphics");
        Debug.Assert(Graphics != null);

        NameLabel = GetNode<Label>("Graphics/NamePanel/Name");
        Debug.Assert(NameLabel != null);

        Graphics.SpriteFrames = Type.Frames;
        NameLabel.Text = Type.Name;

        Graphics.Play("walk_front");

        base._Ready();
    }
}
