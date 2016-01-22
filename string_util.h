void sanitize_string(char *input);

/**
 * Receives user input and verifies that only whitelisted characters are used,
 * in this case, numbers;
 * @param input The number-as-string received from the user
 * @return Returns -1 if an invalid character is used, 0 upon success
 */
int sanitize_number(char *input);
