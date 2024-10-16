Program Laba4;

Uses
    SysUtils;

Type
    TErrors = (FailFileOpen = 0, FailFileCreateOrOpen, FailData,
        FailLimitOfData, NotTXTFile);
    TFile = (FileIn, FileOut, FileCons);
    TMatrix = Array Of Array Of Integer;
    TArray = Array Of Integer;

Const
    ERRORS: Array [0 .. 4] Of String =
        ('�� ������� ������� ���� ��� ��������� ������. ���������� ��� ���, �������� ���� � ��� �����.',
        '�� ������� ������� ��� ������� ����. ���������� ��� ���, �������� ���� � ��� �����.',
        '������������ ������ ������. ���������� ��� ���.',
        '���� �������� �� ������������� �������� ������������. ���������� ��� ���.',
        '���������� ����� ������ ���� TXT. ���������� ��� ���.');
    MIN_LIMIT = -100;
    MAX_LIMIT = 100;
    MIN_LIMIT_SIZE = 0;
    MAX_LIMIT_SIZE = 100;

Function CheckNumForLimitError(Num: Integer;
    Const MAX_LIMIT_NUM, MIN_LIMIT_NUM: Integer): Boolean;
Begin
    If (Num > MAX_LIMIT_NUM) Or (Num < MIN_LIMIT_NUM) Then
    Begin
        Write(ERRORS[Ord(FailLimitOfData)]);
        CheckNumForLimitError := True;
    End
    Else
        CheckNumForLimitError := False;
End;

Function InWithChecking(Var InputFile: TextFile; Var Num: Integer;
    Const MAX_LIMIT_NUM, MIN_LIMIT_NUM: Integer; InType: Integer): Boolean;
Var
    IsFail, isGood: Boolean;
Begin
    repeat
        IsGood := true;
        IsFail := False;

        If EoF(InputFile) Then
        Begin
            Write(ERRORS[Ord(FailData)]);
            IsFail := True;
        End
        Else
        Begin
            Try
                Read(InputFile, Num);
            Except
                Write(ERRORS[Ord(FailData)]);
                IsFail := True;
            End;
        End;

        If Not IsFail Then
            IsFail := CheckNumForLimitError(Num, MAX_LIMIT_NUM, MIN_LIMIT_NUM);

        If (IsFail) And (InType = 1) Then
            isGood := false;

        If IsFail Then
            InWithChecking := True
        Else
            InWithChecking := False;
    until IsGood;
End;


Procedure StrToLow(Var Str: String);
Var
    I, AsciiValue: Integer;
Begin
    For I := 1 To High(Str) Do
    Begin
        AsciiValue := Ord(Str[I]);
        If (AsciiValue > 64) And (AsciiValue < 91) Then
            Str[I] := Char(AsciiValue + 32);
    End;
End;

Function CheckFileNotTxt(PathToFile: String): Boolean;
Var
    StringLength: Integer;
Begin
    StrToLow(PathToFile);

    StringLength := Length(PathToFile);

    If StringLength < 4 Then
        CheckFileNotTxt := True
    Else
    Begin
        If (PathToFile[StringLength] = 't') And
            (PathToFile[StringLength - 1] = 'x') And
            (PathToFile[StringLength - 2] = 't') And
            (PathToFile[StringLength - 3] = '.') Then
            CheckFileNotTxt := False
        Else
            CheckFileNotTxt := True;
    End;
End;

Procedure OpenFile(Var OpendFile: TextFile; FileType: TFile);
Var
    PathToFile: String;
    IsOk: Boolean;
Begin
    repeat
        IsOk := True;

        If FileType = FileIn Then
            Writeln(#10#13, '������� ���� � ����� .txt ��� ����� ������: ')
        Else
            Writeln(#10#13, '������� ���� � ����� .txt ��� ������ ������: ');
        Readln(PathToFile);

        If CheckFileNotTxt(PathToFile) Then
        Begin
            Writeln(ERRORS[Ord(NotTXTFile)]);
            IsOk := False;
        End;

        If IsOk Then
            Try
                AssignFile(OpendFile, PathToFile);
                If FileType = FileIn Then
                    Reset(OpendFile)
                Else
                    Rewrite(OpendFile);
            Except
                If FileType = FileIn Then
                    Writeln(ERRORS[Ord(FailFileOpen)])
                Else
                    Writeln(ERRORS[Ord(FailFileCreateOrOpen)]);

                IsOk := False;
            End;
    until isOk;
End;

Procedure InDataWithConsole(Var InputFile: TextFile; Var M, N: Integer;
    Var Matrix: TMatrix);
Var
    I, J: Integer;
Begin
    AssignFile(InputFile, '');
    Reset(InputFile);

    Writeln('������� ����������� 2M ���������� �������: ');
    InWithChecking(InputFile, M, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE, 1);

    M := M * 2;
    N := M;

    SetLength(Matrix, M);
    For I := 0 To High(Matrix) Do
    Begin
        SetLength(Matrix[I], N);
        For J := 0 To High(Matrix[I]) Do
        Begin
            Write('������� ������� ������� [', I + 1, ', ', J + 1, ']: ');
            InWithChecking(InputFile, Matrix[I][J], MAX_LIMIT, MIN_LIMIT, 1);
        End;
    End;

    Close(InputFile);
End;

Function InDataWithFile(Var InputFile: TextFile; Var M, N: Integer;
    Var Matrix: TMatrix): Boolean;
Var
    I, J: Integer;
    IsNotEnd: Boolean;
Begin
    IsNotEnd := True;
    OpenFile(InputFile, FileIn);

    If InWithChecking(InputFile, M, MAX_LIMIT_SIZE, MIN_LIMIT_SIZE, 2) Then
    Begin
        InDataWithFile := False;
    End;

    M := M * 2;
    N := M;

    SetLength(Matrix, M);
    For I := 0 To High(Matrix) Do
    Begin
        SetLength(Matrix[I], N);
        For J := 0 To High(Matrix[I]) Do
            If (IsNotEnd) And
                (InWithChecking(InputFile, Matrix[I][J], MAX_LIMIT,
                MIN_LIMIT, 2)) Then
            Begin
                InDataWithFile := False;
                IsNotEnd := False;
            End;
    End;

    If IsNotEnd Then
        InDataWithFile := True;
    Close(InputFile);
End;

Procedure InData(Var M, N: Integer; Var Matrix: TMatrix; InType: Integer);
Var
    InputFile: TextFile;
    IsDataOk: Boolean;
Begin
    If InType = 1 Then
        InDataWithConsole(InputFile, M, N, Matrix)
    Else
        Repeat
            IsDataOk := InDataWithFile(InputFile, M, N, Matrix);
        Until IsDataOk;
End;

Procedure FindSeddlePoints(Var Matrix: TMatrix; M, N, OutType: Integer);
Var
    MinRows, MaxColumns: TArray;
    I, J, CountOfAnswers: Integer;
    OutputFile: TextFile;
Begin
    If (OutType = 2) Then
        OpenFile(OutputFile, FileOut);

    SetLength(MinRows, M);
    SetLength(MaxColumns, N);

    For I := 0 To High(MinRows) Do
        MinRows[I] := MAX_LIMIT + 1;

    For I := 0 To High(MaxColumns) Do
        MaxColumns[I] := MIN_LIMIT - 1;

    For I := 0 To High(Matrix) Do
        For J := 0 To High(Matrix[I]) Do
        Begin
            If (Matrix[I, J] < MinRows[I]) Then
                MinRows[I] := Matrix[I, J];
            If (Matrix[I, J] > MaxColumns[J]) Then
                MaxColumns[J] := Matrix[I, J];
        End;

    CountOfAnswers := 0;
    Writeln;
    For I := 0 To High(MinRows) Do
        For J := 0 To High(MaxColumns) Do
            If MinRows[I] = MaxColumns[J] Then
            Begin
                Inc(CountOfAnswers);
                Writeln(MinRows[I], ' - [', I + 1, ',', J + 1, ']');
                If OutType = 2 Then
                    Writeln(OutputFile, MinRows[I], ' - [', I + 1, ',',
                        J + 1, ']');
            End;

    If CountOfAnswers = 0 Then
    Begin
        Writeln('� ������ ������� ��� �������� �����.');
        If OutType = 2 Then
            Writeln(OutputFile, '� ������ ������� ��� �������� �����.');
    End;

    Writeln;
    If OutType = 2 Then
        Close(OutputFile);
End;

Procedure ChangeMatrix(Var Matrix: TMatrix; M, N: Integer);
Var
    NewMatrix: TMatrix;
    I, J, Middle: Integer;
Begin
    SetLength(NewMatrix, M);
    For I := 0 To High(Matrix) Do
        SetLength(NewMatrix[I], N);

    Middle := N Div 2;

    For I := 0 To High(Matrix) Do
        For J := 0 To High(Matrix[I]) Do
            If I < Middle Then
                If J < Middle Then
                    NewMatrix[I + Middle, J + Middle] := Matrix[I, J]
                Else
                    NewMatrix[I + Middle, J - Middle] := Matrix[I, J]
            Else
                If J < Middle Then
                    NewMatrix[I - Middle, J] := Matrix[I, J]
                Else
                    NewMatrix[I - Middle, J] := Matrix[I, J];

    For I := 0 To N - 1 Do
        For J := 0 To N - 1 Do
            Matrix[I, J] := NewMatrix[I, J];
End;

Procedure OutMatrix(Matrix: TMatrix);
Var
    I, J: Integer;
Begin
    Writeln;
    For I := 0 To High(Matrix) Do
    Begin
        For J := 0 To High(Matrix[I]) Do
            Write(Matrix[I, J], ' ');
        Writeln;
    End;
    Writeln;
End;

Procedure ExitProgram();
Begin
    Writeln('��� ������ �� ��������� ������� Enter...');
    Readln;
End;

Var
    N, M, InType, OutType: Integer;
    Matrix: TMatrix;
    ConsoleFile: TextFile;

Begin
    Writeln('��������� ��� ��������� ��������� ��������� ���������� ������� ������� 2n.');

    Writeln(#10#13, '��� �������� ������� ������ ���� �� ', MIN_LIMIT, ' �� ',
        MAX_LIMIT, '.');
    Writeln('������� ���������� ������� 2M ���� �� ', MIN_LIMIT_SIZE, ' �� ',
        MAX_LIMIT_SIZE, '.');

    Writeln(#10#13, '������� �������������� ��� ����� ������: ');
    Writeln(#9, '1 - �� ������� (�� ��������),', #10#13#9,
        '2 - �� ����� (���� ������ m � n, ������ �������� � ���� �������).');

    AssignFile(ConsoleFile, '');
    Reset(ConsoleFile);
    InWithChecking(ConsoleFile, InType, 2, 1, 1);

    InData(M, N, Matrix, InType);

    OutMatrix(Matrix);

    Writeln('������� �������������� ��� ������ ������: ');
    Writeln(#9, '1 - ������ � �������,', #10#13#9, '2 - � ������� � � ����.');

    InWithChecking(ConsoleFile, OutType, 2, 1, 1);
    Close(ConsoleFile);

    ChangeMatrix(Matrix, M, N);

    OutMatrix(Matrix);

    ExitProgram();

End.
