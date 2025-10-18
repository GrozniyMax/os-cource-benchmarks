public class HelloWorld {
    public static void main(String[] args) {

        char[] letters = {'H', 'e', 'l', 'l', 'o', ',', ' ', 'W', 'o', 'r', 'l', 'd', '!'};

        String base = "";
        for (char c: letters) {
            base += c;
        }

        System.out.println(base);
    }
}