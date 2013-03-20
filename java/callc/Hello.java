public class Hello
{
    public native void hello();

    static
    {
	System.loadLibrary("hello");
    }

    public static void main(String[] args)
    {
	Hello hello = new Hello();
	hello.hello();
    }
}