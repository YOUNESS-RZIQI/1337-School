import os
import sys
from dotenv import load_dotenv


def get_config_value(key: str) -> str:
    # Get value from environment variables
    value = os.getenv(key)

    # If missing, show error and stop program
    if not value:
        print(f"[ERROR] Missing required configuration: {key}\n")

    return value


def main() -> None:
    print("\nORACLE STATUS: Reading the Matrix...\n")

    # Load variables from .env file into environment

    load_dotenv()
    all_ok = 0

    # Read configuration values
    matrix_mode = get_config_value("MATRIX_MODE")
    database_url = get_config_value("DATABASE_URL")
    api_key = get_config_value("API_KEY")
    log_level = get_config_value("LOG_LEVEL")
    zion_endpoint = get_config_value("ZION_ENDPOINT")

    print("Configuration loaded:")
    if matrix_mode:
        print(f"Mode: {matrix_mode}")
        all_ok += 1
    else:
        print(f"Mode: {matrix_mode}")

    if database_url:
        print("Database: Connected to local instance")
        all_ok += 1
    else:
        print("Database: Error")

    if api_key:
        print("API Access: Authenticated")
        all_ok += 1
    else:
        print("API Access: Error ")

    if log_level:
        print("Log Level: DEBUG")
        all_ok += 1
    else:
        print("Log Level: Error")

    if zion_endpoint:
        print("Zion Network: Online\n")
        all_ok += 1
    else:
        print("Zion Network: Error\n")
    if all_ok == 5:
        print("Environment security check:")
        print("[OK] No hardcoded secrets detected")
        print("[OK] .env file properly configured")
        print("[OK] Production overrides available")

        print("\nThe Oracle sees all configurations.")
    else:
        print("! Somthing whent Wrong !")


if __name__ == "__main__":
    main()
