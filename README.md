# A simple experimental database

> Nothin in the world can take the place of persistence. - Calvin Coolidge

## Architecture
   ![github](./assets/architecture.png) <br><br>
   The frontend layer only have things that client view. <br>
   The backend layer have all neccessary processing to a storage data (disk operation, btree...).  <br>

## Contributions
   Forke (or no) the project and open PR. Only. It can be directly on the master.

   **Code style** <br>
   All header files must contains #ifndef _HEADER_FILE_ and #define _HEADER_FILE_ after, to avoid multiple imports.<br>
   All variable and functions must contains small latters and words separated  with _ (underline).<br>

   I understand that in your web project you use words separated by capital letters, however, in C all commands are in snake_case, which in itself is a good reason to separate words with _ (underline). Furthermore, which of the two strings below do you find more comfortable to read:
   ```
   IHadAGreenDollWithALargeTShirt
   
   i_had_a_green_doll_with_a_large_t_shirt  

   ```

   The ifs must contains spacings between operations: `a != b`, although dont after parentheses: `ìf(a != b)`.<br>
   The keys must declared on the same line as the function: `void function() {` and must contains space after the parentheses.<br>
   The identation must a 4 spaces. Configure your code editor. <br>
