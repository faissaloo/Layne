// Copyright (C) 2017 Faissal Bensefia
//
// This file is part of Layne.
//
// Layne is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Layne is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Layne.  If not, see <http://www.gnu.org/licenses/>.

unsigned int uint_len(unsigned int n)
{
	if (n < 100000)
	{
		// 5 or less
		if (n < 100){
			// 1 or 2
			if (n < 10)
			{
				return 1;
			}
			else
			{
				return 2;
			}
		}
		else
		{
			// 3 or 4 or 5
			if (n < 1000)
			{
				return 3;
			}
			else
			{
				// 4 or 5
				if (n < 10000)
				{
					return 4;
				}
				else
				{
					return 5;
				}
			}
		}
	}
	else
	{
		// 6 or more
		if (n < 10000000)
		{
			// 6 or 7
			if (n < 1000000)
			{
				return 6;
			}
			else
			{
				return 7;
			}
		}
		else
		{
			// 8 to 10
			if (n < 100000000)
			{
				return 8;
			}
			else
			{
				// 9 or 10
				if (n < 1000000000)
				{
					return 9;
				}
				else
				{
					return 10;
				}
			}
		}
	}
}

int int_len(int n)
{
	if (n < 0)
	{
		return uint_len(n)+1;
	}
	else
	{
		return uint_len(n);
	}
}

void int_to_str(int n,char *buff)
{
	if (n<0)
	{
		*buff++='-';
		n=-n;
	}

	buff+=uint_len(n);
	while (n!=0)
	{
		buff--;
		//On x86 DIV (integer divide) outputs both remainder and the rest so this should optimise to just one instruction unless some other trickery gets done
		*buff='0'+(n%10);
		n/=10;
	}
}
