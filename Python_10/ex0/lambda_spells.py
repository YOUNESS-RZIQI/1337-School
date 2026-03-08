from typing import Union


def artifact_sorter(artifacts: list[dict]) -> list[dict]:
    return sorted(artifacts, key=lambda a: a["power"], reverse=True)


def power_filter(mages: list[dict], min_power: int) -> list[dict]:
    return list(filter(lambda m: m["power"] >= min_power, mages))


def spell_transformer(spells: list[str]) -> list[str]:
    return list(map(lambda s: f"* {s} *", spells))


def mage_stats(mages: list[dict]) -> dict[str, Union[int, float]]:
    powers = list(map(lambda m: m["power"], mages))

    return {
        "max_power": max(powers),
        "min_power": min(powers),
        "avg_power": round(sum(powers) / len(powers), 2)
    }


def labda_sanctum() -> None:
    artifacts = [
        {"name": "Crystal Orb", "power": 85, "type": "orb"},
        {"name": "Fire Staff", "power": 92, "type": "staff"},
        {"name": "Shadow Blade", "power": 78, "type": "blade"},
    ]

    print("\nTesting artifact sorter...")

    sorted_artifacts = artifact_sorter(artifacts)
    print(f"{sorted_artifacts[0]['name']} ({sorted_artifacts[0]['power']}"
          " power)"
          f" comes before {sorted_artifacts[1]['name']}"
          f" ({sorted_artifacts[1]['power']} power)\n")

    spells = ["fireball", "heal", "shield"]
    print("Testing spell transformer...")
    print(' '.join(spell_transformer(spells)))


if __name__ == "__main__":

    try:
        labda_sanctum()
    except KeyError as e:
        print("Key Error:", e)
    except Exception as e:
        print("Error:", e)
