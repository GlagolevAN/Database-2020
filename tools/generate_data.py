import random
import string

DEFAULT_FILENAME = "data.txt"

def generate_random_string(min_len=6, max_len=20):
    """Generates a random string of English letters."""
    # Define the character set to choose from (lowercase and uppercase letters)
    characters = string.ascii_letters
    
    length = random.randint(min_len, max_len)
    
    # Create the random string by choosing characters one by one
    random_str = ''.join(random.choice(characters) for _ in range(length))
    return random_str

def generate_10_digit_integer():
    """Generates a random 7-digit positive integer."""
    return random.randint(1_000_000, 9_999_999)

def generate_special_integer():
    """Generates a random integer from one of the specified ranges."""
    ranges = [
        (101, 112),
        (201, 212),
        (301, 312),
        (401, 412)
    ]
    
    chosen_range = random.choice(ranges)
    
    # Generate a random integer within the selected range (inclusive)
    return random.randint(chosen_range[0], chosen_range[1])

def create_data_file(filename, num_lines):
    """Creates a text file with the specified number of lines and format."""
    try:
        with open(filename, 'w') as f:
            for i in range(num_lines):
                item1 = generate_random_string()
                item2 = generate_10_digit_integer()
                item3 = generate_special_integer()
                
                line = f"{item1} {item2} {item3}\n"
                
                f.write(line)
        print(f"\nSuccessfully created '{filename}' with {num_lines} lines.")
    except IOError as e:
        print(f"\nError: Could not write to file '{filename}'. Reason: {e}")

# --- Main execution block ---
if __name__ == "__main__":
    print("--- Data File Generator ---")
    
    # Get user input for filename
    output_filename = input("Enter the desired output filename (e.g., data.txt): ")
    if not output_filename:
        output_filename = DEFAULT_FILENAME

    # Get user input for the number of lines
    while True:
        try:
            lines_to_generate = int(input("How many lines of data do you want to generate? "))
            if lines_to_generate > 0:
                break
            else:
                print("Please enter a positive number.")
        except ValueError:
            print("Invalid input. Please enter a whole number.")
            
    create_data_file(output_filename, lines_to_generate)

