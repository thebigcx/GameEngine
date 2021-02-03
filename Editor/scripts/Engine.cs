using System.Runtime.CompilerServices;

namespace Engine
{

public class Vector3
{
    public Vector3()
    {
        x = 0;
        y = 0;
        z = 0;
    }

    public float x, y, z;
};

public class Vector4
{
    public Vector4()
    {
        x = 0;
        y = 0;
        z = 0;
        w = 0;
    }

    public float x, y, z, w;
};

public class Matrix4
{
    private Vector4[] m_cells;
};

public class NativeBinding
{

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern void getCameraComponent_Native(ulong gameObjectID, out CameraComponent component);

    [MethodImplAttribute(MethodImplOptions.InternalCall)]
    public static extern void getTransformComponent_Native(ulong gameObjectID, out TransformComponent component);

};

public abstract class GameComponent
{

};

public class TransformComponent : GameComponent
{
    public TransformComponent()
    {
        translation = new Vector3();
        rotation = new Vector3();
        scale = new Vector3();
    }

    public Vector3 translation;
    public Vector3 rotation;
    public Vector3 scale;
};

public class CameraComponent : GameComponent
{

};

public abstract class GameObject
{
    public T getComponent<T>() where T : GameComponent
    {
        if (typeof(T) == typeof(CameraComponent))
        {
            return (T)(GameComponent)getCameraComponent();
        }
        else if (typeof(T) == typeof(TransformComponent))
        {
            return (T)(GameComponent)getTransformComponent();
        }

        return null;
    }

    public CameraComponent getCameraComponent()
    {
        CameraComponent component;
        NativeBinding.getCameraComponent_Native(0, out component);
        return component;
    }

    public TransformComponent getTransformComponent()
    {
        TransformComponent component;
        NativeBinding.getTransformComponent_Native(0, out component);
        return component;
    }
};

}