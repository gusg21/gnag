using System;

[AttributeUsage(AttributeTargets.Method, AllowMultiple = false)]
public class DebugActionAttribute : Attribute {
    public string ButtonName;
}