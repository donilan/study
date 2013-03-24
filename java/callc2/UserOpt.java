public class UserOpt{
    public native User findUser(String username);
    public native void saveUser(User u);

    static {
        System.loadLibrary("UserOpt");
    }
}