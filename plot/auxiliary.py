def formatText(text: str) -> str:
    words = text.split()
    result = []
    line = ""

    for word in words:
        if len(line) + len(word) + 1 <= 50:
            if line:
                line += " "
            line += word
        else:
            result.append(line)
            line = word

    if line:
        result.append(line)

    return "\n".join(result)
