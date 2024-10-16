Program Project1;

{$APPTYPE CONSOLE}
{$R *.res}

Uses
    System.SysUtils;

Type
    TError = (FailData, FailLimitOfData, FigureError);

Const
    ERRORS: Array [0 .. 2] Of String =
        ('������������ ������. ���������� ��� ���. ',
        '���� �������� �� ������������� �������� ������������. ���������� ��� ���. ',
        '������ ��������� ������������ �� �������� ����������\����');
    MIN_LIMIT = -1000;
    MAX_LIMIT = 1000;

Var
    I, CountOfPoints, VectorSum: Integer;
    SignOfVectorSum, Sign: Integer;
    FirstIndex, SecondIndex, ThirdIndex: Integer;
    FirstLineX, FirstLineY, SecondLineX, SecondLineY: Integer;
    IsFail, IsEndOfFor: Boolean;
    PointX, PointY: Array Of Integer;

Begin
    Writeln('��������� ��� �����������, �������� �� ������������� �������� ��� ���?');

    Write(#10#13, '������� ���������� ������ �������������� �� ', 3, ' �� ',
        MAX_LIMIT, ': ');
    Repeat
        IsFail := False;

        Try
            Readln(CountOfPoints);
        Except
            Write(ERRORS[Ord(FailData)]);
            IsFail := True;
        End;

        If Not IsFail And ((CountOfPoints > MAX_LIMIT) Or
            (CountOfPoints < 3)) Then
        Begin
            Write(ERRORS[Ord(FailLimitOfData)]);
            IsFail := True;
        End;
    Until Not IsFail;

    SetLength(PointX, CountOfPoints);
    SetLength(PointY, CountOfPoints);

    for I := 0 to High(PointX) do
    Begin
        Write('������� ���������� ������� (x, y) �� ', MIN_LIMIT, ' �� ',
            MAX_LIMIT, ' #', I + 1, ': ');
        Repeat
            IsFail := False;

            Try
                Readln(PointX[I], PointY[I]);
            Except
                Write(ERRORS[Ord(FailData)]);
                IsFail := True;
            End;

            If Not IsFail And
                ((PointX[I] > MAX_LIMIT) Or (PointX[I] < MIN_LIMIT) Or
                (PointY[I] > MAX_LIMIT) Or (PointY[I] < MIN_LIMIT)) Then
            Begin
                Write(ERRORS[Ord(FailLimitOfData)]);
                IsFail := True;
            End;
        Until Not IsFail;
    End;

    IsEndOfFor := False;
    Sign := -2;

    For I := 0 To High(PointX) Do
    Begin
        FirstIndex := I;
        SecondIndex := (I + 1) Mod CountOfPoints;
        ThirdIndex := (I + 2) Mod CountOfPoints;

        FirstLineX := PointX[SecondIndex] - PointX[FirstIndex];
        FirstLineY := PointY[SecondIndex] - PointY[FirstIndex];
        SecondLineX := PointX[ThirdIndex] - PointX[SecondIndex];
        SecondLineY := PointY[ThirdIndex] - PointY[SecondIndex];

        VectorSum := (FirstLineX * SecondLineY - SecondLineX * FirstLineY);

        If VectorSum = 0 Then
            SignOfVectorSum := 0
        Else
            SignOfVectorSum := VectorSum Div Abs(VectorSum);

        If SignOfVectorSum <> 0 Then
            If Sign = -2 Then
                Sign := SignOfVectorSum
            Else
                If Sign <> SignOfVectorSum Then
                    IsEndOfFor := True;
    End;

    If Sign = -2 Then
        Writeln(ERRORS[Ord(FigureError)])
    Else
        If IsEndOfFor Then
            Writeln('������������� �� �������� ��������.')
        Else
            Writeln('������������� �������� ��������.');

    Writeln('��� ������ �� ��������� ������� Enter...');
    Readln;
End.
