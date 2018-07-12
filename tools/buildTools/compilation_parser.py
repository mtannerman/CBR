import sys

def is_error_line(line):
    if ": error:" in line:
        return True
    return False

def is_note_line(line):
    if ": note:" in line:
        return True
    return False

def parse_line(line, nextLine):
    words = line.split()
    if len(words) < 4:
        return None
    splitFirstLine = words[0].split(':')
    if len(splitFirstLine) < 2:
        return None
    ret = []
    ret.append(parse_file(splitFirstLine[0]))
    ret.append(splitFirstLine[1]) 
    ret.append(splitFirstLine[2]) 
    ret.append(parse_description(' '.join(words[2::]), nextLine))
    return ret

def parse_file(f):
    return '/'.join(f.split('/')[-3::])

def parse_description(desc, nextLine):
    if "no matching function" in desc:
        return "Non existent function: " + " ".join(desc.split()[6::])
    elif "no viable conversion" in desc:
        apostropheSplitting = desc.split('\'')
        return "No conversion: " + apostropheSplitting[1] + " => " + apostropheSplitting[3]
    elif "return type of out-of-line" in desc:
        return "Bad return type: " + desc.split('\'')[1]
    elif "no matching constructor for initialization of" in desc:
        return "No matching constructor for type: " + desc.split('\'')[3]
    elif "expected expression" in desc:
        return "expected expression: " + nextLine.strip()
    
    return desc

def is_critical_log(line):
    if "Built target" in line:
        return True
    return False

def parse_error_lines(compilationLogContent):
    errorLines = []
    for i in range(len(compilationLogContent)):
        line  = compilationLogContent[i]
        nextLine = None if i == (len(compilationLogContent) - 1) else compilationLogContent[i + 1]
        if is_error_line(line):
            errorLines.append(parse_line(line, nextLine))

    return errorLines
