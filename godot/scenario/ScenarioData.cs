using Godot;

[GlobalClass]
public partial class ScenarioData : Resource {
    [Export(PropertyHint.Range, "1,100")]
    public int GridWidth;
    [Export(PropertyHint.Range, "1,100")]
    public int GridHeight;
    [Export]
    public Godot.Collections.Array<ScenarioCharacterInstanceData> CharacterInstances;

    public ScenarioData() {
        GridWidth = 10;
        GridHeight = 10;
        CharacterInstances = new();
    }
}