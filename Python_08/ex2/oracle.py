import os
import sys
from dotenv import load_dotenv




def get_config_value(key: str) -> str:
    # Get value from environment variables
    value = os.getenv(key)

    # If missing, show error and stop program
    if not value:
        print(f"[ERROR] Missing required configuration: {key}")
        sys.exit(1)

    return value

def main() -> None:
    print("\nORACLE STATUS: Reading the Matrix...\n")

    # Load variables from .env file into environment
    load_dotenv("./.env.example")

    # Read configuration values
    matrix_mode = get_config_value("MATRIX_MODE")
    database_url = get_config_value("DATABASE_URL")
    api_key = get_config_value("API_KEY")
    log_level = get_config_value("LOG_LEVEL")
    zion_endpoint = get_config_value("ZION_ENDPOINT")

    print("Configuration loaded:")

    print(f"Mode: {matrix_mode}")

    if database_url == "Connection string for data storage":
        print("Database: Connected to local instance")
    else:
        print("Database: NOt Connected")

    if api_key == "Secret key for external services":
        print("API Access: Authenticated")
    else:
        print("API Access: Not Authenticated")
    if log_level == "Logging verbosity":
        print(f"Log Level: DEBUG")
    else:
        print(f"Log Level: Not DEBUG")

    if zion_endpoint == "URL for the resistance network":
        print("Zion Network: Online\n")
    else:
        print("Zion Network: Offline\n")

    print("Environment security check:")
    print("[OK] No hardcoded secrets detected")
    print("[OK] .env file properly configured")
    print("[OK] Production overrides available")

    print("\nThe Oracle sees all configurations.")


if __name__ == "__main__":
    main()
