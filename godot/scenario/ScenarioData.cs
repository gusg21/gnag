using System;
using Godot;

[GlobalClass]
public partial class ScenarioData : Resource {
    [Export(PropertyHint.Range, "1,100")]
    public int GridWidth;
    [Export(PropertyHint.Range, "1,100")]
    public int GridHeight;

    public ScenarioData() : this(10, 10) {}

    public ScenarioData(int gridWidth, int gridHeight) {
        GridWidth = gridWidth;
        GridHeight = gridHeight;
    }
}