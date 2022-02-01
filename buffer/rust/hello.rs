fn main() {
	println!("HW");  // macro!?
	println!("{} days", 31);
	println!("{0}, this is {1}. {1}, this is {0}", "Alice", "Bob");
	println!("{subj} {verb} {pred}",
		pred="ow",
		subj="su",
		verb="jump");
}
