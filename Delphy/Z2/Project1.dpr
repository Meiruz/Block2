Program Project1;

{$APPTYPE CONSOLE}
{$R *.res}

Uses
    System.SysUtils;

Type
    TError = (FailData, FailLimitOfData);
    TArray = Array Of Integer;

Const
    ERRORS: Array [0 .. 1] Of String =
        ('������������ ������. ���������� ��� ���. ',
        '���� �������� �� ������������� �������� ������������. ���������� ��� ���. ');
    MIN_LIMIT = 1;
    MAX_LIMIT = 2000000000;

Procedure InWithChecking(Var Value: Integer);
Var
    IsGood: Boolean;
Begin
    Repeat
        IsGood := True;

        Try
            Readln(Value);
        Except
            IsGood := False;
            Write(ERRORS[Ord(FailData)]);
        End;

        If IsGood And ((Value > MAX_LIMIT) Or (Value < MIN_LIMIT)) Then
        Begin
            IsGood := false;
            Write(ERRORS[Ord(FailLimitOfData)]);
        End;
    Until IsGood;
End;

Function GetNumLength(Num: Integer): Integer;
Var
    Length: Integer;
Begin
    Length := 0;

    If Num = 0 Then
        GetNumLength := 1;

    While Num > 0 Do
    Begin
        Num := Num Div 10;
        Inc(Length);
    End;

    GetNumLength := Length;
End;

Procedure SplitNumToArray(Num, NumLength: Integer; Var ArrNum: TArray);
Begin
    While Num > 0 Do
    Begin
        Dec(NumLength);
        ArrNum[NumLength] := Num Mod 10;
        Num := Num Div 10;
    End;
End;

Procedure OutElement(Element: Char; Count: Integer;
    IsStartEndl, IsEndEndl: Boolean);
Var
    I: Integer;
Begin
    If IsStartEndl Then
        Writeln;

    For I := 1 To Count Do
        Write(Element);

    If IsEndEndl Then
        Writeln;
End;

Procedure DivisonOfElements(Var ArrFirstNum: TArray;
    FirstNum, SecondNum, FirstNumLength: Integer);
Var
    Pos, UnderlinedNum, AddUnderlinedRange, UnderlinedNumLength,
        CheckPos: Integer;
    DivisionAns, RoundDivision, DivisionPos, NewUnderlinedRange: Integer;
    IsNotFirst: Boolean;
Begin
    Pos := 0;
    UnderlinedNum := 0;
    AddUnderlinedRange := 1;
    CheckPos := 0;
    UnderlinedNumLength := 0;
    IsNotFirst := False;

    Writeln(#10#13, FirstNum, ' | ', SecondNum);
    While CheckPos < FirstNumLength Do
    Begin
        While (CheckPos < FirstNumLength) And (UnderlinedNum < SecondNum) Do
        Begin
            UnderlinedNum := UnderlinedNum * 10 + ArrFirstNum[CheckPos];
            Inc(AddUnderlinedRange);
            Inc(CheckPos);
        End;
        Dec(AddUnderlinedRange);

        If IsNotFirst Then
        Begin
            OutElement(' ', Pos, False, False);
            OutElement('-', AddUnderlinedRange, False, True);
            OutElement(' ', Pos, False, False);
            Writeln(UnderlinedNum);
        End;

        DivisionAns := UnderlinedNum Div SecondNum;
        RoundDivision := DivisionAns * SecondNum;
        DivisionPos := Pos + (AddUnderlinedRange - GetNumLength(RoundDivision));

        OutElement(' ', DivisionPos, False, False);
        Write(RoundDivision);

        If IsNotFirst Then
            Writeln
        Else
        Begin
            OutElement(' ', FirstNumLength - AddUnderlinedRange, False, False);
            Writeln(' | ', FirstNum Div SecondNum);
        End;

        Dec(UnderlinedNum, RoundDivision);

        NewUnderlinedRange := GetNumLength(UnderlinedNum);
        Pos := Pos + AddUnderlinedRange - NewUnderlinedRange;
        AddUnderlinedRange := NewUnderlinedRange + 1;

        If UnderlinedNum = 0 Then
        Begin
            Inc(Pos);
            Dec(AddUnderlinedRange);
        End;

        IsNotFirst := True;
        CheckPos := Pos + AddUnderlinedRange - 1;
    End;

    OutElement(' ', Pos, False, False);
    OutElement('-', AddUnderlinedRange - 1, False, True);
    OutElement(' ', Pos, False, False);

    Writeln(UnderlinedNum, #10#13);

End;

Procedure ExitProgram();
Begin
    Writeln('��� ������ �� ��������� ������� Enter...');
    Readln;
End;

Var
    I, FirstNum, SecondNum: Integer;
    FirstNumLength: Integer;
    IsGood: Boolean;
    ArrNum: TArray;

Begin

    Writeln('��������� ��� ������� ����� � �������. ');

    Write(#10#13, '������� �������� �������� �� ', MIN_LIMIT, ' �� ',
        MAX_LIMIT, ': ');
    InWithChecking(FirstNum);

    Write(#10#13, '������� �������� �������� �� ', MIN_LIMIT, ' �� ',
        MAX_LIMIT, ': ');
    InWithChecking(SecondNum);

    FirstNumLength := GetNumLength(FirstNum);
    SetLength(ArrNum, FirstNumLength);
    SplitNumToArray(FirstNum, FirstNumLength, ArrNum);

    DivisonOfElements(ArrNum, FirstNum, SecondNum, FirstNumLength);

    ExitProgram();

End.
