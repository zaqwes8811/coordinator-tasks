package testing

/**
 * Created by zaqwes on 3/29/15.
 */
//fun main(args : Array<String>) {
//    println("Hello, world!")
//}

//fun main(args : Array<String>) {
//    if (args.size == 0) {
//        println("Please provide a name as a command-line argument")
//        return
//    }
//    println("Hello, ${args[0]}!")
//}

/*fun main(args : Array<String>) {
    for (name in args)
        println("Hello, $name!")
}*/

fun main(args: Array<String>) {
    val language = if (args.size == 0) "EN" else args[0]
    println(when (language) {
        "EN" -> "Hello!"
        "FR" -> "Salut!"
        "IT" -> "Ciao!"
        "seyfer" -> "seed!"
        else -> "Sorry, I can't greet you in $language yet"
    })
}