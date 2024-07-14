public class MoveAction : IBoardAction {
    public Character Target;

    public MoveAction(Character target) {
        Target = target;
    }

    public string GetHelperInfo() {
        return "Select a tile to move to";
    }
}