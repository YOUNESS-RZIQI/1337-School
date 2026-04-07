from typing import Any, Callable


def spell_combiner(spell1: Callable, spell2: Callable) -> Callable:
    def combined(*args: Any, **kwargs: Any) -> tuple:
        return (spell1(*args, **kwargs), spell2(*args, **kwargs))
    return combined


def power_amplifier(base_spell: Callable, multiplier: int) -> Callable:
    def amplified(*args: Any, **kwargs: Any) -> int | float:
        return base_spell(*args, **kwargs) * multiplier
    return amplified


def conditional_caster(condition: Callable, spell: Callable) -> Callable:
    def caster(*args: Any, **kwargs: Any) -> Any:
        if condition(*args, **kwargs):
            return spell(*args, **kwargs)
        return "Spell fizzled"
    return caster


def spell_sequence(spells: list[Callable]) -> Callable:
    def sequence(*args: Any, **kwargs: Any) -> list:
        return [spell(*args, **kwargs) for spell in spells]
    return sequence


def higher_magic() -> None:
    def fireball(target: str) -> str:
        return f"Fireball hits {target}"

    def heal(target: str) -> str:
        return f"Heals {target}"

    print("\nTesting spell combiner...")
    combined = spell_combiner(fireball, heal)
    r1, r2 = combined("Dragon")
    print(f"Combined spell result: {r1}, {r2}")

    def damage(target: str) -> int:
        return 10

    print("\nTesting power amplifier...")
    mega = power_amplifier(damage, 3)
    print(f"Original: {damage('Dragon')}, Amplified: {mega('Dragon')}")


if __name__ == "__main__":
    try:
        higher_magic()
    except TypeError as e:
        print("Type Error :", e)
    except Exception as e:
        print("Error :", e)
