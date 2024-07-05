using System;
using Godot;

[GlobalClass]
public partial class CharacterType : Resource {
    [Export]
    public string Name;
    [Export]
    public SpriteFrames Frames;
    [Export]
    public bool PlayerControlled;

    public CharacterType() {
        Name = "Unnamed Character";
        Frames = ResourceLoader.Load<SpriteFrames>("res://character/sf_unknown.tres");
        PlayerControlled = false;
    }
}