package com.createw.jnitest;

public class Main {
	public static void main(String[] args) {
		User u = new User();
		u.setUsername("doni");
		u.setPassword("test");
		new UserOpt().saveUser(u);
		System.out.println("User: " + new UserOpt().getUser("test").getUsername());
		
	}
}
