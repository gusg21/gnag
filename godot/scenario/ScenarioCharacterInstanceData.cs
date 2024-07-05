using Godot;

[GlobalClass]
public partial class ScenarioCharacterInstanceData : Resource {
    [Export]
    public CharacterType CharacterType;
    [Export]
    public Vector2I TilePos;

    public ScenarioCharacterInstanceData() {
        CharacterType = ResourceLoader.Load<CharacterType>("res://character/ct_unknown.tres");
        TilePos = new Vector2I(2, 2);
    }
}